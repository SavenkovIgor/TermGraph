import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Button {
    id: btn
//    width: 400
    font.pixelSize: mainObj.getUiElementSize("button")*Screen.pixelDensity;
    implicitHeight: mainObj.getUiElementSize("button")*Screen.pixelDensity*2.2;

    leftPadding: 20
    rightPadding: 20

}
