import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

import "../Js/Colors.js" as Colors

Button {
    rightPadding: 12
    leftPadding: 12
    bottomPadding: 10
    topPadding: 10

    flat: true

    property alias labelText: label.text
    property alias iconSource: icon.icon.source

    property string baseColor: Colors.white

    onEnabledChanged: {
        baseColor = Colors.whiteDisabled
    }

    onBaseColorChanged: {
        icon.color = baseColor
        label.color = baseColor
    }

    contentItem: Row {

        spacing: icon.width / 5

        MyRoundButton {
            id: icon
            color: Colors.white
            backgroundHidden: true

            icon.width: label.font.pixelSize * 1.8
            icon.height: label.font.pixelSize * 1.8
        }

        Text {
            id: label
            height: icon.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            color: Colors.white

            font.weight: Font.Thin
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }
    }
}
