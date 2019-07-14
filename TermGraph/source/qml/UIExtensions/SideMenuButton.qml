import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Button {
    rightPadding: 12
    leftPadding: 12
    bottomPadding: 10
    topPadding: 10

    flat: true

    property alias labelText: label.text
    property alias iconName: icon.iconName

    property string baseColor: appColors.white

    onEnabledChanged: {
        baseColor = appColors.whiteDisabled
    }

    onBaseColorChanged: {
        icon.color = baseColor
        label.color = baseColor
    }

    contentItem: Row {

        spacing: icon.width / 5

        MyRoundButton {
            id: icon
            color: appColors.white
            backgroundHidden: true

            icon.width: label.font.pixelSize * 1.8
            icon.height: label.font.pixelSize * 1.8
        }

        Text {
            id: label
            height: icon.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            color: appColors.white

            font.weight: Font.Thin
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }
    }
}
