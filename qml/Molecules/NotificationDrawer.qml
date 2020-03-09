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

    function setTextAndOpen(text, startTimer = true) {
        infoLabel.text = text;
        root.open();
        if (startTimer)
            hideTimer.start();
    }

    function showDebug(debugInfo) { setTextAndOpen("Debug: " + debugInfo, false); }
    function showError(error)     { setTextAndOpen("Ошибка: " + error);           }
    function showWarning(warning) { setTextAndOpen("Предупреждение: " + warning); }
    function showInfo(info)       { setTextAndOpen("Информация: " + info);        }

    Connections {
        target: Notification
        onShowInfoQml: root.showInfo(info)
        onShowWarningQml: root.showWarning(warning)
        onShowErrorQml: root.showError(error)
        onShowDebugQml: root.showDebug(debugInfo)
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

    Timer {
        id: hideTimer
        interval: 3000
        repeat: false
        onTriggered: {
            if (infoLabel.lineCount == 1) // Auto close only if label is small
                root.close()
        }
    }
}
