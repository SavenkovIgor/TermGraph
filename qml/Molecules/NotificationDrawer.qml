import QtQuick 2.14
import QtQuick.Controls 2.14

import "../Js/Sizes.js" as Sizes
import "../Js/Fonts.js" as Fonts

import Notification 1.0

Drawer {
    id: root

    implicitHeight: topPadding + implicitContentHeight + bottomPadding
    interactive: position !== 0.0
    edge: Qt.BottomEdge
    dim: false

    onOpened: Notification.handleNotifyShow();
    onClosed: Notification.handleNotifyHide();

    Component.onCompleted: {
        Qt.callLater(function() {
            Notification.handleUiInitialization();
        });
    }

    function setTextAndOpen(text) {
        infoLabel.text = text;
        root.open();
    }

    function showDebug(debugInfo) { setTextAndOpen("Debug: " + debugInfo);        }
    function showError(error)     { setTextAndOpen("Ошибка: " + error);           }
    function showWarning(warning) { setTextAndOpen("Предупреждение: " + warning); }
    function showInfo(info)       { setTextAndOpen("Информация: " + info);        }

    Connections {
        target: Notification
        onShowInfoQml: root.showInfo(info)
        onShowWarningQml: root.showWarning(warning)
        onShowErrorQml: root.showError(error)
        onShowDebugQml: root.showDebug(debugInfo)
        onHideNotify: root.close();
    }

    contentItem: TextArea {
        id: infoLabel

        text: ""
        readOnly: true
        wrapMode: TextEdit.Wrap

        horizontalAlignment: Text.AlignHCenter
        font: Fonts.inputText

        leftPadding: Sizes.baseR50
        rightPadding: Sizes.baseR50
    }
}
