import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Column {
    property alias labelText: label.text
    property alias placeholderText: txtField.placeholderText
    property alias text: txtField.text

    anchors {
        left: parent.left
        right: parent.right
    }

    function takeFocus() {
        txtField.forceActiveFocus()
    }

    Label {
        id: label
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity

        color: "#e8e8e8"
    }

    TextField {
        id : txtField
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
        selectByMouse: true

        color: "#e8e8e8"
        placeholderTextColor: "#9c9c9c"
    }
}
