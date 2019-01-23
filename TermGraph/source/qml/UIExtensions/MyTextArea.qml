import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Column {

    property alias labelText: label.text
    property alias placeholderText:  txtArea.placeholderText
    property alias text: txtArea.text
    property alias txtFocus: txtArea.focus

    anchors {
        left: parent.left
        right: parent.right
    }

    function makeFocusInField() {
        txtArea.forceActiveFocus()
    }

    function getCursPosition() {
        return txtArea.cursorPosition
    }

    function setCursPosition(pos) {
        txtArea.cursorPosition = pos
    }

    Label {
        id: label
        anchors {
            left: parent.left
            right: parent.right
        }

        color: "#e8e8e8"

        font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity
    }

    TextArea {
        id: txtArea
        anchors {
            left: parent.left
            right: parent.right
        }

        color: "#e8e8e8"
        placeholderTextColor: "#9c9c9c"

        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity

        wrapMode: TextEdit.WordWrap

    }
}
