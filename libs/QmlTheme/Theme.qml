// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQml
import QtQuick

QtObject {
    id: root

    readonly property QtObject color: QtObject {

        readonly property color stdTint: "#45000000"

        readonly property QtObject term: QtObject {
            readonly property color leafNode   : "#83c31b"
            readonly property color rootNode   : "#dc4c1d"
            readonly property color orphanNode : "#0b77b1"
        }

        readonly property QtObject edge: QtObject {
            readonly property color standard       : "black"
            readonly property color termin         : "#225b7a"
            readonly property color terminHardLink : "#a091b5"
            readonly property color description    : "green"
            readonly property color selected       : "#00a693"
            readonly property color selectedAlt    : "#dc4c1d"
            readonly property color broken         : "red"
        }

        readonly property color black         : "#000000"
        readonly property color white         : "#e8e8e8"
        readonly property color whiteDisabled : "#9c9c9c"
        readonly property color base          : "#332f30"
        readonly property color baseLight     : "#4f4f4f"
        readonly property color baseLight2    : "#464544"
        readonly property color baseLight3    : "#6f6f6f"
        readonly property color accent        : "#2f7ba3"
        readonly property color warning       : "#e5b41f"

        readonly property ColorGroup group : ColorGroup {
            text:          root.color.white
            base:          root.color.base
            alternateBase: root.color.baseLight
            highlight:     root.color.accent
        }
    }

    readonly property QtObject font: QtObject {
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
}
