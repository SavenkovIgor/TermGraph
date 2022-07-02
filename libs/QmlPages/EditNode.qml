// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import StyleInfo

M.Page {
    id: root

    title: "Изменить термин"

    StackView.onActivating: updateInfo()

    header: M.DefaultHeader {

        A.ToolButton {
            icon.source: IconPath.trash
            onClicked: nodeDelDialog.open()
        }

        A.ToolButton {
            icon.source: IconPath.check
            onClicked: root.applyNodeChange()
        }
    }

    signal termEdited(string termName)

    M.StdDialog {
        id: nodeDelDialog

        title: "Удалить этот термин?"

        onAccepted: {
            root.StackView.view.pop();
            Scene.deleteSelectedTerm();
        }
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.applyNodeChange(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.applyNodeChange(); }

    function updateInfo() {
        nodeUuidText.text = Scene.currentNode.uuid;
        lastEditText.text = Scene.currentNode.lastEditDecorated;
        termName.text = Scene.currentNode.term;
        termDefin.text = Scene.currentNode.definition;
        termDescr.text = Scene.currentNode.description;
        termExampl.text = Scene.currentNode.examples;
        nodeGroup.selectElement(Scene.currentNode.groupUuid);
        termDefin.takeFocus();
    }

    function applyNodeChange() {
        if (termName.text === "") {
            emptyNodeNameDelDialog.visible = true;
            return;
        }

        root.termEdited(termName.text); // Before node change. It is important

        const data = {
            uuid: nodeUuidText.text,
            term: termName.text,
            definition: termDefin.text,
            description: termDescr.text,
            examples: termExampl.text,
            wikiUrl: "",
            wikiImage: "",
            groupUuid: nodeGroup.currentText,
            lastEdit: Scene.currentNode.lastEdit
        };

        const success = GroupsManager.updateNode(data);

        if (success) {
            root.StackView.view.pop();
        }
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

        M.LabelPair {
            id: nodeUuidText
            name: "Uuid термина:"
            textObj.wrapMode: Text.NoWrap
            textObj.minimumPixelSize: Fonts.h4.pixelSize / 2
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

            A.AccentText { text : "Группа:" }

            A.ComboBox {
                id: nodeGroup
                Layout.fillWidth: true

                model: GroupsManager.allUuidSorted

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

    M.StdDialog {
        id: emptyNodeNameDelDialog
        title: "Заполните поле 'Термин'"
        standardButtons: M.StdDialog.Ok
    }
}
