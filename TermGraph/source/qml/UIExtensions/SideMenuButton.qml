import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

Button {
    flat: true
    font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
    height: Math.floor( font.pixelSize * 2.7 )
    font.weight: Font.Medium
}
