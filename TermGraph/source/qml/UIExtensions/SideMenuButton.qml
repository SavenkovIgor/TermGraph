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

    property string baseColor: "#e8e8e8"

    onEnabledChanged: {
        baseColor = "#55e8e8e8"
    }

    onBaseColorChanged: {
        icon.color = baseColor
        label.color = baseColor
    }

    contentItem: Row {

        spacing: icon.width / 5

        MyRoundButton {
            id: icon
            property string iconPath: ""

            color: "#e8e8e8"
            backgroundHidden: true

            icon.width: label.font.pixelSize * 1.8
            icon.height: label.font.pixelSize * 1.8

            Component.onCompleted: loadIcon(iconPath)
        }

        Text {
            id: label
            height: icon.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            color: "#e8e8e8"

            font.weight: Font.Thin
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }
    }
}
