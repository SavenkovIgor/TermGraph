// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Api

import Theme

C.ComboBox {
    id: control
    font: Theme.font.h4

    contentItem: Text {
        id: contItem
        text: Api.groups.getGroupName(control.displayText)
        font: control.font
        color: Theme.color.base7
        padding: font.pixelSize / 1.5
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle { color: Theme.color.base1 }

    popup: C.Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: C.ScrollIndicator { }
        }

        background: Rectangle {
            border { color: Theme.color.base1; width: 2; }
            color: Theme.color.base0
            radius: 2
        }
    }

    //Item in the popup
    delegate: C.ItemDelegate {
        width: control.width
        contentItem: Text {
            text: Api.groups.getGroupName(modelData)
            font: control.font
            color: Theme.color.base7
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        highlighted: control.highlightedIndex === index
    }
}
