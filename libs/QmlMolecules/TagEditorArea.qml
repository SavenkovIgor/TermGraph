// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Api

import Molecules as M

M.TextArea {
    id: root

    labelText: "Определение:"
    placeholderText: "- это"
    inputMethodHints: Qt.ImhNoAutoUppercase

    readonly property bool isCursorOnLink: {
        // Cursor can be invalid in moment of text editing
        let pos = cursorPosition;
        if (TagProcessor.isValidCursor(text, pos))
            return TagProcessor.isCursorOnLink(text, pos);

        return false;
    }

    readonly property bool addLinkButtonVisible: txtFocus && !isCursorOnLink
    readonly property bool extendLinkButtonVisible: txtFocus && isCursorOnLink
    readonly property bool removeLinkButtonVisible: txtFocus && isCursorOnLink

    function addLink() {
        let pos = cursorPosition;

        if (!isCursorOnLink) {
            text = TagProcessor.addLink(text, pos);
            takeFocusAndSetCursor(pos + 1);
        } else {
            takeFocusAndSetCursor(pos);
        }
    }

    function expandLinkRight() {
        let pos = cursorPosition;
        if (isCursorOnLink) {
            text = TagProcessor.expandLinkRight(text, pos);
            takeFocusAndSetCursor(pos);
        }
    }

    function removeLink() {
        let pos = cursorPosition;
        if (isCursorOnLink) {
            text = TagProcessor.removeLink(text, pos);
            takeFocusAndSetCursor(pos - 1);
        }
    }

    // This function need because taking focus drops cursor position
    function takeFocusAndSetCursor(cursorPos) {
        takeFocus();
        cursorPosition = cursorPos;
    }
}
