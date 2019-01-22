import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

Button {
    rightPadding: 12
    leftPadding: 12
    bottomPadding: 10
    topPadding: 10

    flat: true

    property alias labelText: label.text
    property alias iconPath: icon.iconPath

    property string baseColor: "#FF0000"

    onBaseColorChanged: {
        icon.color = baseColor
        label.color = baseColor
    }

    contentItem: Row {

        spacing: icon.width / 5

        MyRoundButton {
            id: icon
            property string iconPath: ""

            icon.width: label.font.pixelSize * 1.8
            icon.height: label.font.pixelSize * 1.8

            Component.onCompleted: {
                loadIcon(iconPath)
                hideBack()
            }
        }

        Text {
            id: label
            height: icon.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.weight: Font.Thin
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }
    }
}
