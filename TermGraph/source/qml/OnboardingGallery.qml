import QtQuick 2.5
import QtQuick.Controls 2.5

Page {
    id: onboarding

    property StackView mainStack
    function open() {
        mainStack.push(onboarding)
    }

    Rectangle {
        anchors.fill: parent
        color: "#4f4f4f"

        Rectangle {
            anchors.fill: parent
            anchors.margins: {
                left: width/10
                right: width/10
                top: height/10
                bottom: height/10
            }
            color: "#7f7f7f"
        }
    }

}
