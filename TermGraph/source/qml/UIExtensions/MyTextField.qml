import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Column {
    property alias labelText: label.text
    property alias placeholderText: txtField.placeholderText
    property alias text: txtField.text

    signal newText

    anchors { left: parent.left; right: parent.right; }  // TODO: delete This

    function takeFocus() {
        txtField.forceActiveFocus()
    }

    Label {
        id: label
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity

        color: appColors.accent
    }

    TextField {
        id : txtField
        width: parent.width
        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
        selectByMouse: true

        onTextChanged: newText()

        color: appColors.white
        placeholderTextColor: appColors.whiteDisabled
    }
}
