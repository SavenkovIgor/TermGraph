// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Molecules as M
import StyleInfo

M.Page {
    id: root

    padding: 0
    title: "Справка"

    contentItem: M.HelpView { }
}
