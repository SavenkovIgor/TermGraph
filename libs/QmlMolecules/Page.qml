// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import StyleInfo

Page {
    id: root

    signal openMainMenu()

    padding: Math.min(width, height) * 0.035
    background: null

    header: DefaultHeader {
        onOpenMainMenu: root.openMainMenu()
    }
}
