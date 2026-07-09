// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme

ToolBar {
    height: Theme.sizes.baseX4
    background: Rectangle { color: Theme.color.base2; }
    contentItem: RowLayout { spacing: 20; }
}
