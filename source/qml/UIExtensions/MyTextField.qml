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
        anchors {
            left: parent.left
            right: parent.right
        }
        font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity
    }

    TextField {
        id : txtField
        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
        anchors {
            left: parent.left
            right: parent.right
        }

        selectByMouse: true
    }
}
