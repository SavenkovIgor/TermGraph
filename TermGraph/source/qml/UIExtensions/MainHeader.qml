import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

ToolBar {
    //        Material.foreground: "white"
    id: mainHeader

    signal menuClick

    property alias titleText: titleLabel.text

    function showMenuIcon() {
        mainMenuButton.loadIcon("qrc:/icons/menu")
    }

    function showArrowIcon() {
        mainMenuButton.loadIcon("qrc:/icons/arrow-thick-left")
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent

        MyRoundButton {
            id: mainMenuButton

            Component.onCompleted: {
//                loadIcon( "qrc:/icons/menu")
                hideBack()
            }

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
    }
}
