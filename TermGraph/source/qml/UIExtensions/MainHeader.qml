import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

ToolBar {
    id: mainHeader

    signal menuClick
    signal checkClick

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

            Component.onCompleted: {
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

        MyRoundButton {
            id: checkButton
            visible: false

            Component.onCompleted: {
                hideBack()
                loadIcon("qrc:/icons/check")
            }

            onClicked: mainHeader.checkClick()
        }
    }
}
