// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import StyleInfo

Text {
    // Info
    // TODO: Make required, when groupsModel appear
    /*required */property string description
    /*required */property string label

    text: description + ": " + label

    // Padding
    topPadding: Math.max(font.pixelSize / 4, 1)
    leftPadding: font.pixelSize * 3
    bottomPadding: Math.max(font.pixelSize / 4, 1)

    // Font
    font: {
        let f = Fonts.h4;
        f.pixelSize *= 0.7;
        f.weight = Font.Thin;
        return f;
    }

    // Alignment
    verticalAlignment:  Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    color: Colors.white
}