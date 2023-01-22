// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme

ToolBar {
    height: Sizes.baseX4
    background: Rectangle { color: Theme.color.base2; }
    contentItem: RowLayout { spacing: 20; }
}
