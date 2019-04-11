import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

ToolBar {
    id: mainHeader

    signal menuClick
    signal checkClick

    background: Rectangle {
        anchors.fill: parent
        color: "#4f4f4f"
    }

    property alias titleText: titleLabel.text

    function showMenuIcon() {
        mainMenuButton.loadIcon("qrc:/icons/menu")
    }

    function showArrowIcon() {
        mainMenuButton.loadIcon("qrc:/icons/arrow-thick-left")
    }

    function showCheckButton() {
        checkButton.visible = true
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent

        MyRoundButton {
            id: mainMenuButton
            color: "#e8e8e8"
            backgroundHidden: true
            onClicked: mainHeader.menuClick()
        }

        Label {
            id: titleLabel

            font.pixelSize: mainObj.getUiElementSize("appHeader") * Screen.pixelDensity
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }

        MyRoundButton {
            id: checkButton
            visible: false
            color: "#e8e8e8"
            backgroundHidden: true

            Component.onCompleted: loadIcon("qrc:/icons/check")

            onClicked: mainHeader.checkClick()
        }
    }
}
