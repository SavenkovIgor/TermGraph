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

import QtQuick 2.15

import Helpers 1.0

import Molecules 1.0 as M

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
