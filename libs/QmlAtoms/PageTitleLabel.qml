// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Theme

Text {
    font: Theme.font.h1
    minimumPixelSize: font.pixelSize / 2
    fontSizeMode: Text.Fit
    elide: Text.ElideRight
    color: Theme.color.base7
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
}
