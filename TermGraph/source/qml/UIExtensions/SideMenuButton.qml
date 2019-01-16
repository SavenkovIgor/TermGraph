import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

Button {

    property alias label: buttonLabel.text
    property alias iconPath: image.source

    flat: true

    width: image.width + buttonLabel.width + padding * 3 + 10
    height: image.height + padding * 2

    contentItem: Item {
        id: element

        Image {
            id: image
            width: height
            height: buttonLabel.font.pixelSize * 2
        }

        Text {
            id: buttonLabel
            text: "asdsssf"
            verticalAlignment: Text.AlignVCenter
            width: image.height * 10

            anchors {
                verticalCenter: image.verticalCenter
                left: image.right
                leftMargin: 10
            }

            font.weight: Font.Medium
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }
    }
}
