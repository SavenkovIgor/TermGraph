/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15

import Atoms 1.0 as A
import Molecules 1.0 as M
import StyleInfo 1.0

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