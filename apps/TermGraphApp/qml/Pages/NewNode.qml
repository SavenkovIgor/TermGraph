/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import StyleInfo

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
        currentGroupFixedRow.groupUuid = Scene.currentGroup.uuid;
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

        const data = {
            uuid: "",
            term: termName.text,
            definition: termDefin.text,
            description: termDescr.text,
            examples: termExampl.text,
            wikiUrl: "",
            wikiImage: "",
            groupUuid: currentGroupFixedRow.groupUuid,
            lastEdit: ""
        };

        const result = GroupsManager.addNode(data);

        if (result)
            root.StackView.view.pop();
    }

    property bool someOfToolButtonsHaveFocus: addLink.focus || expandLinkRight.focus || removeLink.focus

    A.RoundButton {
        id: addLink
        icon.source: IconPath.code
        visible: termDefin.addLinkButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: expandLinkRight.left; margins: width / 2; }

        onClicked: termDefin.addLink()
    }

    A.RoundButton {
        id: expandLinkRight
        icon.source: IconPath.chevronRight
        visible: termDefin.extendLinkButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: parent.right; margins: width / 2; }

        onClicked: termDefin.expandLinkRight()
    }

    A.RoundButton {
        id: removeLink
        icon.source: IconPath.code
        icon.color: "crimson"
        visible: termDefin.removeLinkButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: expandLinkRight.left; margins: width / 2; }

        onClicked: termDefin.removeLink()
    }

    contentItem: A.FlickableColumn {

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

                text: GroupsManager.getGroupName(currentGroupFixedRow.groupUuid)
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

    M.StdDialog {
        id: emptyNodeNameDelDialog
        title: "Заполните поле 'Термин'"
        standardButtons: M.StdDialog.Ok
    }
}
