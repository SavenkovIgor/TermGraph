// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Theme

C.Page {
    id: root

    signal openMainMenu()

    padding: Math.min(width, height) * 0.035
    background: null

    header: DefaultHeader {
        onOpenMainMenu: root.openMainMenu()
    }
}
