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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import StyleInfo

Text {
    // Info
    // TODO: Make required, when groupsModel appear
    /*required */property string description
    /*required */property string label

    text: description + ": " + label

    // Padding
    topPadding: Math.max(font.pixelSize / 4, 1)
    leftPadding: font.pixelSize * 3
    bottomPadding: Math.max(font.pixelSize / 4, 1)

    // Font
    font: {
        let f = Fonts.text;
        f.pixelSize *= 0.7;
        f.weight = Font.Thin;
        return f;
    }

    // Alignment
    verticalAlignment:  Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    color: Colors.white
}
