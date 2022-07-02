// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Api

import StyleInfo

ComboBox {
    id: control
    font: Fonts.h4

    contentItem: Text {
        id: contItem
        text: GroupsManager.getGroupName(control.displayText)
        font: control.font
        color: Colors.white
        padding: font.pixelSize / 1.5
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle { color: Colors.baseLight2 }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: Colors.black
            radius: 2
        }
    }

    //Item in the popup
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: GroupsManager.getGroupName(modelData)
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        highlighted: control.highlightedIndex === index
    }
}
