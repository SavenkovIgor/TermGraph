// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme
import Atoms as A

Button {
    id: root

    horizontalPadding: Sizes.base
    verticalPadding: Sizes.halfBase
    topInset: 0
    bottomInset: 0

    flat: true

    readonly property bool iconVisible: {
        return display == AbstractButton.IconOnly || display == AbstractButton.TextBesideIcon
    }
    readonly property bool textVisible: {
        return display == AbstractButton.TextOnly || display == AbstractButton.TextBesideIcon
    }

    display: AbstractButton.TextBesideIcon

    property color baseColor: {
        if (!enabled)
            return Colors.whiteDisabled;

        return highlighted ? Colors.baseLight3 : Colors.white
    }

    contentItem: Row {
        spacing: Sizes.halfBase

        A.ToolButton {
            size: Sizes.baseX3
            icon.color: root.baseColor
            icon.source: root.icon.source
            visible: root.iconVisible
        }

        Text {
            height: Sizes.baseX3

            color: root.baseColor
            font: Fonts.setWeight(Fonts.h3, Font.Thin)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: root.text
            visible: root.textVisible
        }
    }
}
