import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

Button {
    id: btn
//    width: 400
    font.pixelSize: mainObj.getUiElementSize("button")*Screen.pixelDensity;
    implicitHeight: mainObj.getUiElementSize("button")*Screen.pixelDensity*2.2;

    leftPadding: 20
    rightPadding: 20

}
