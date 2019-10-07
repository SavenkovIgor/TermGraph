import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

import "../Js/iconpath.js" as IconPath

ToolBar {
    id: mainHeader

    signal menuClick
    signal checkClick

    background: Rectangle {
        anchors.fill: parent
        color: appColors.baseLight
    }

    property alias title: titleLabel.text

    function showMenuIcon()  { mainMenuButton.icon.source = IconPath.menu }
    function showArrowIcon() { mainMenuButton.icon.source = IconPath.leftArrow }

    function showCheckButton() {
        checkButton.visible = true
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent

        MyRoundButton {
            id: mainMenuButton
            color: appColors.white
            backgroundHidden: true
            onClicked: mainHeader.menuClick()
        }

        Label {
            id: titleLabel

            font.pixelSize: mainObj.getUiElementSize("appHeader") * Screen.pixelDensity
            minimumPixelSize: font.pixelSize / 2
            fontSizeMode: Text.Fit
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }

        MyRoundButton {
            id: checkButton
            icon.source: IconPath.check
            visible: false
            color: appColors.white
            backgroundHidden: true

            onClicked: mainHeader.checkClick()
        }
    }
}
