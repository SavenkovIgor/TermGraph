import QtQuick 2.14

import "../Molecules" as M

M.TextArea {
    id: root

    readonly property bool isInsideTag: {
        // Cursor can be invalid in moment of text editing
        let pos = cursorPosition;
        if (tagProcessor.isValidCursor(text, pos))
            return tagProcessor.isInsideTag(text, pos);

        return false;
    }

    readonly property bool addTagButtonVisible: txtFocus && !isInsideTag
    readonly property bool extendTagButtonVisible: txtFocus && isInsideTag
    readonly property bool removeTagButtonVisible: txtFocus && isInsideTag

    function addTag() {
        let pos = cursorPosition;

        if (!isInsideTag) {
            text = tagProcessor.addTag(text, pos);
            takeFocusAndSetCursor(pos + 1);
        } else {
            takeFocusAndSetCursor(pos);
        }
    }

    function expandTagRight() {
        let pos = cursorPosition;
        if (isInsideTag) {
            text = tagProcessor.expandTagRight(text, pos);
            takeFocusAndSetCursor(pos);
        }
    }

    function removeTag() {
        let pos = cursorPosition;
        if (isInsideTag) {
            text = tagProcessor.removeTag(text, pos);
            takeFocusAndSetCursor(pos - 1);
        }
    }

    // This function need because taking focus drops cursor position
    function takeFocusAndSetCursor(cursorPos) {
        takeFocus();
        cursorPosition = cursorPos;
    }
}
