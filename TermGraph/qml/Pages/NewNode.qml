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

    property string defaultUuidText: "startValue"

    StackView.onActivating: prepareForOpen()

    title: "Новый термин"

    header: M.DefaultHeader {

        A.ToolButton {
            id: checkButton
            icon.source: IconPath.check
            onClicked: root.addNode()
        }
    }

    signal newTermAdded(string termName)

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.addNode(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.addNode(); }

    function prepareForOpen() {
        currentGroupFixedRow.groupUuid = scene.currentGroup;
        clearText(termName);
        clearText(termDefin);
        clearText(termDescr);
        clearText(termExampl);
        termName.takeFocus();
    }

    function clearText(textItem) { textItem.text = ""; }

    function addNode() {
        if (termName.text === "") {
            emptyNodeNameDelDialog.visible = true;
            return;
        }

        root.newTermAdded(termName.text); // Before node add. It is important

        var success = nodesManager.addNewNode(
                    termName.text,
                    termDefin.text,
                    termDescr.text,
                    termExampl.text,
                    currentGroupFixedRow.groupUuid
                    );

        if (success)
            root.StackView.view.pop();
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

    contentItem: A.VerticalFlickable {
        id: flick

        contentWidth:  contentColumn.width
        contentHeight: contentColumn.height

        ColumnLayout {
            id: contentColumn

            spacing: mainObj.getUiElementSize("colSpace")
            width: flick.width

            RowLayout {
                id: currentGroupFixedRow

                property string groupUuid: ""

                spacing: 6
                Layout.fillWidth: true

                A.AccentText { text: "Группа:" }

                Text {
                    Layout.fillWidth: true
                    leftPadding: 15

                    font: Fonts.text
                    wrapMode: TextEdit.NoWrap
                    elide: Text.ElideRight
                    color: Colors.white

                    text: groupsManager.getGroupName(currentGroupFixedRow.groupUuid)
                }
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
