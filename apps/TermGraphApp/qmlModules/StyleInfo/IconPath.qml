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

pragma Singleton

import QtQml

QtObject {
    readonly property string book            : "qrc:/book.svg"
    readonly property string check           : "qrc:/check.svg"
    readonly property string chevronLeft     : "qrc:/chevron-left.svg"
    readonly property string chevronRight    : "qrc:/chevron-right.svg"
    readonly property string code            : "qrc:/code.svg"
    readonly property string cog             : "qrc:/cog.svg"
    readonly property string crossOnCircle   : "qrc:/crossOnCircle.svg"
    readonly property string eye             : "qrc:/eye.svg"
    readonly property string folder          : "qrc:/folder.svg"
    readonly property string info            : "qrc:/info.svg"
    readonly property string leftArrow       : "qrc:/arrow-thick-left.svg"
    readonly property string list            : "qrc:/list.svg"
    readonly property string loopCircular    : "qrc:/loop-circular.svg"
    readonly property string magnifyingGlass : "qrc:/magnifying-glass.svg"
    readonly property string menu            : "qrc:/menu.svg"
    readonly property string options         : "qrc:/options.svg"
    readonly property string pencil          : "qrc:/pencil.svg"
    readonly property string plus            : "qrc:/plus.svg"
    readonly property string questionMark    : "qrc:/question-mark.svg"
    readonly property string rightArrow      : "qrc:/arrow-thick-right.svg"
    readonly property string share           : "qrc:/share.svg"
    readonly property string spreadsheet     : "qrc:/spreadsheet.svg"
    readonly property string trash           : "qrc:/trash.svg"
    readonly property string warning         : "qrc:/warning.svg"
}
