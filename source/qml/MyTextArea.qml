import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Column {

    property alias labelText: label.text
    property alias placeholderText:  txtArea.placeholderText
    property alias text: txtArea.text
    property alias txtFocus: txtArea.focus

    width: parent.width

    function makeFocusInField() {
        txtArea.forceActiveFocus()
    }

    function getCursPosition() {
        return txtArea.cursorPosition
    }

    Label {
        id: label
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity
    }

    TextArea {
        id: txtArea
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity

        wrapMode: TextEdit.WordWrap

    }
}
