// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Molecules as M
import Theme

M.Page {
    id: root

    padding: 0
    title: "Справка"

    contentItem: M.HelpView { }
}
