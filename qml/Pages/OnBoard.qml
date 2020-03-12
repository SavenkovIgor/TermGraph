/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

import QtQuick 2.14
import QtQuick.Layouts 1.14

import "../Atoms" as A
import "../Molecules" as M
import "../Js/IconPath.js" as IconPath
import "../Js/Sizes.js" as Sizes

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