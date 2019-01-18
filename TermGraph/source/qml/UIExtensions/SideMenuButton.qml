import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

Button {
    rightPadding: 12
    leftPadding: 12
    bottomPadding: 10
    topPadding: 10
    spacing: icon.width / 2
    flat: true

    icon.width: font.pixelSize * 1.8
    icon.height: font.pixelSize * 1.8

    font.weight: Font.Thin
    font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
}
