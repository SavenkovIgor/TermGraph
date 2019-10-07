import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

import "../Js/Colors.js" as Colors

Column {

    property alias labelText: label.text
    property alias placeholderText:  txtArea.placeholderText
    property alias text: txtArea.text
    property alias txtFocus: txtArea.focus
    property alias cursorPosition: txtArea.cursorPosition

    function takeFocus() {
        txtArea.forceActiveFocus()
        txtArea.cursorPosition = txtArea.text.length
    }

    Label {
        id: label
        width: parent.width

        color: Colors.accent
        font.pixelSize: mainObj.getUiElementSize("inputLabel") * Screen.pixelDensity
    }

    TextArea {
        id: txtArea
        color: Colors.white
        width: parent.width
        placeholderTextColor: Colors.whiteDisabled

        font.pixelSize: mainObj.getUiElementSize("inputText") * Screen.pixelDensity
        wrapMode: TextEdit.WordWrap
    }
}
