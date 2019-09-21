import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

ColumnLayout {

    property alias name: nameLbl.text
    property alias text: textLbl.text

    spacing: 6

    Label {
        id: nameLbl
        font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity + 1
        font.weight: Font.DemiBold

        color: appColors.accent

        Layout.fillWidth: true
    }

    Label {
        id: textLbl
        font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity
        wrapMode: TextEdit.WordWrap

        anchors.leftMargin: 30

        color: appColors.white

        Layout.fillWidth: true
    }
}
