// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts

import Atoms as A
import Molecules as M
import Theme

M.Page {
    id: root

    signal finished()

    header: A.PageHeader {

        A.PageTitleLabel { Layout.fillWidth: true }

        A.ToolButton {
            icon.source: IconPath.check
            visible: helpView.lastSlide
            onClicked: root.finished()
        }
    }

    contentItem: M.HelpView { id: helpView; focus: true; }
}
