// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Api

import Theme

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

        function onShowInfoQml(info)       { root.showInfo(info) }
        function onShowWarningQml(warning) { root.showWarning(warning) }
        function onShowErrorQml(error)     { root.showError(error) }
        function onShowDebugQml(debugInfo) { root.showDebug(debugInfo) }
        function onHideNotify()            { root.close() }
    }

    contentItem: TextArea {
        id: infoLabel

        text: ""
        readOnly: true
        wrapMode: TextEdit.Wrap

        horizontalAlignment: Text.AlignHCenter
        font: Fonts.h3

        leftPadding: Sizes.baseX2
        rightPadding: Sizes.baseX2
    }
}
