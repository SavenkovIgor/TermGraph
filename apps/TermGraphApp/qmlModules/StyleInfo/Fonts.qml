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
    readonly property font h1 : Qt.font({ pixelSize: 5   * Sizes.rm, weight: 30 });
    readonly property font h2 : Qt.font({ pixelSize: 4   * Sizes.rm });
    readonly property font h3 : Qt.font({ pixelSize: 3   * Sizes.rm });
    readonly property font h4 : Qt.font({ pixelSize: 2.5 * Sizes.rm });

    readonly property font term : Qt.font({ family: "Arial sans-serif", pointSize: 10 });

    function setWeight(font, weight) {
        font.weight = weight;
        return font;
    }
}
