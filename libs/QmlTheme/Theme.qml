// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

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

    readonly property QtObject icon: QtObject {
        readonly property url book            : "qrc:/qt/qml/Theme/icon/book.svg"
        readonly property url check           : "qrc:/qt/qml/Theme/icon/check.svg"
        readonly property url chevronLeft     : "qrc:/qt/qml/Theme/icon/chevron-left.svg"
        readonly property url chevronRight    : "qrc:/qt/qml/Theme/icon/chevron-right.svg"
        readonly property url code            : "qrc:/qt/qml/Theme/icon/code.svg"
        readonly property url cog             : "qrc:/qt/qml/Theme/icon/cog.svg"
        readonly property url crossOnCircle   : "qrc:/qt/qml/Theme/icon/crossOnCircle.svg"
        readonly property url eye             : "qrc:/qt/qml/Theme/icon/eye.svg"
        readonly property url folder          : "qrc:/qt/qml/Theme/icon/folder.svg"
        readonly property url info            : "qrc:/qt/qml/Theme/icon/info.svg"
        readonly property url leftArrow       : "qrc:/qt/qml/Theme/icon/arrow-thick-left.svg"
        readonly property url list            : "qrc:/qt/qml/Theme/icon/list.svg"
        readonly property url loopCircular    : "qrc:/qt/qml/Theme/icon/loop-circular.svg"
        readonly property url magnifyingGlass : "qrc:/qt/qml/Theme/icon/magnifying-glass.svg"
        readonly property url menu            : "qrc:/qt/qml/Theme/icon/menu.svg"
        readonly property url options         : "qrc:/qt/qml/Theme/icon/options.svg"
        readonly property url pencil          : "qrc:/qt/qml/Theme/icon/pencil.svg"
        readonly property url plus            : "qrc:/qt/qml/Theme/icon/plus.svg"
        readonly property url questionMark    : "qrc:/qt/qml/Theme/icon/question-mark.svg"
        readonly property url rightArrow      : "qrc:/qt/qml/Theme/icon/arrow-thick-right.svg"
        readonly property url share           : "qrc:/qt/qml/Theme/icon/share.svg"
        readonly property url spreadsheet     : "qrc:/qt/qml/Theme/icon/spreadsheet.svg"
        readonly property url trash           : "qrc:/qt/qml/Theme/icon/trash.svg"
        readonly property url warning         : "qrc:/qt/qml/Theme/icon/warning.svg"
    }
}
