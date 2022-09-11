// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
