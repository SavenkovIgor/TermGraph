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
        if (LinkTools.isValidCursor(text, pos))
            return LinkTools.isCursorOnLink(text, pos);

        return false;
    }

    readonly property bool addLinkButtonVisible: txtFocus && !isCursorOnLink
    readonly property bool extendLinkButtonVisible: txtFocus && isCursorOnLink
    readonly property bool removeLinkButtonVisible: txtFocus && isCursorOnLink

    function addLink() {
        let pos = cursorPosition;

        if (!isCursorOnLink) {
            text = LinkTools.add(text, pos);
            takeFocusAndSetCursor(pos + 1);
        } else {
            takeFocusAndSetCursor(pos);
        }
    }

    function expandLinkRight() {
        let pos = cursorPosition;
        if (isCursorOnLink) {
            text = LinkTools.expandRight(text, pos);
            takeFocusAndSetCursor(pos);
        }
    }

    function removeLink() {
        let pos = cursorPosition;
        if (isCursorOnLink) {
            text = LinkTools.remove(text, pos);
            takeFocusAndSetCursor(pos - 1);
        }
    }

    // This function need because taking focus drops cursor position
    function takeFocusAndSetCursor(cursorPos) {
        takeFocus();
        cursorPosition = cursorPos;
    }
}
