import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

import "../Js/Colors.js" as Colors

Column {
    property alias labelText: label.text
    property alias placeholderText: txtField.placeholderText
    property alias text: txtField.text

    signal newText()

    function takeFocus() {
        txtField.forceActiveFocus()
    }

    Label {
        id: label
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputLabel") * Screen.pixelDensity

        color: Colors.accent
    }

    TextField {
        id : txtField
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputText") * Screen.pixelDensity
        selectByMouse: true

        onTextChanged: newText()

        color: Colors.white
        placeholderTextColor: Colors.whiteDisabled
    }
}
