import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

ColumnLayout {

    property alias name: nameLbl.text
    property alias text: textLbl.text

    spacing: 6

    Label{
        id: nameLbl
        font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity + 1
        font.weight: Font.DemiBold

        Layout.fillWidth: true
    }

    Label {
        id: textLbl
        font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
        wrapMode: TextEdit.WordWrap

        anchors.leftMargin: 30

        Layout.fillWidth: true
    }
}
