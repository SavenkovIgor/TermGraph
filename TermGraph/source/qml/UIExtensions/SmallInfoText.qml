import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "../Js/Colors.js" as Colors

Text {
    // Info
    property string description: ""
    property string label: ""

    text: description + ": " + label

    // Padding
    topPadding: Math.max(font.pixelSize / 4, 1)
    leftPadding: font.pixelSize * 3
    bottomPadding: Math.max(font.pixelSize / 4, 1)

    // Font
    font.weight: Font.Thin
    font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity * 0.7

    // Alignment
    verticalAlignment:  Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    color: Colors.white
}
