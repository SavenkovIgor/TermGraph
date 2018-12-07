import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

Column {

    property alias name: nameLbl.text
    property alias text: textLbl.text

//    height: Math.max(nameLbl.height,textLbl.height)

    spacing: 6

    Label{
        id: nameLbl
        font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity + 1
        font.weight: Font.DemiBold
//        anchors.left: parent.left
    }
    Label {
        id: textLbl
        font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
        wrapMode: TextEdit.WordWrap

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 30

//        onHeightChanged: {
//            parent.height = Math.max(nameLbl.height,textLbl.height)
//        }

    }
}
