// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    id: root

    title: "Изменить термин"

    StackView.onActivating: updateInfo()

    header: M.DefaultHeader {

        A.ToolButton {
            icon.source: Theme.icon.trash
            onClicked: nodeDelDialog.open()
        }

        A.ToolButton {
            icon.source: Theme.icon.check
            onClicked: root.applyNodeChange()
        }
    }

    signal termEdited(string termName)

    M.StdDialog {
        id: nodeDelDialog

        title: "Удалить этот термин?"

        onAccepted: {
            root.StackView.view.pop();
            Api.scene.deleteSelectedTerm();
        }
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.applyNodeChange(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.applyNodeChange(); }

    function updateInfo() {
        nodeUuidText.text = Api.scene.currentNode.uuid;
        lastEditText.text = Api.scene.currentNode.lastEditDecorated;
        termName.text = Api.scene.currentNode.term;
        termDefin.text = Api.scene.currentNode.definition;
        termDescr.text = Api.scene.currentNode.description;
        termExampl.text = Api.scene.currentNode.examples;
        nodeGroup.selectElement(Api.scene.currentNode.groupUuid);
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
            lastEdit: Api.scene.currentNode.lastEdit
        };

        Api.groups.updateNode(data);
    }

    Connections {
        target: Api.groups

        function onTermUpdated() {
            root.StackView.view.pop();
        }
    }

    property bool someOfToolButtonsHaveFocus: addLink.focus || expandLinkRight.focus || removeLink.focus

    A.RoundButton {
        id: addLink
        icon.source: Theme.icon.code
        visible: termDefin.addLinkButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: expandLinkRight.left; margins: width / 2; }

        onClicked: termDefin.addLink()
    }

    A.RoundButton {
        id: expandLinkRight
        icon.source: Theme.icon.chevronRight
        visible: termDefin.extendLinkButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: parent.right; margins: width / 2; }

        onClicked: termDefin.expandLinkRight()
    }

    A.RoundButton {
        id: removeLink
        icon.source: Theme.icon.code
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
            textObj.minimumPixelSize: Theme.font.h4.pixelSize / 2
            textObj.fontSizeMode: Text.HorizontalFit
            Layout.fillWidth: true
        }

        M.TextField {
            id: termName
            Layout.fillWidth: true
            labelText: "Термин:"
        }

        M.LinkEditorArea {
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

                model: Api.groups.allUuidSorted

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
