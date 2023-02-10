// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property QtObject color: QtObject {

        readonly property QtObject term: QtObject {
            readonly property color leafNode   : "#1f5f5f"
            readonly property color rootNode   : "#711e1d"
            readonly property color orphanNode : "#3a586f"
        }

        readonly property QtObject edge: QtObject {
            readonly property color standard       : "black"
            readonly property color termin         : "#00a693"
            readonly property color terminHardLink : "#a091b5"
            readonly property color description    : "green"
            readonly property color selected       : "#00a693"
            readonly property color selectedAlt    : "#a091b5"
            readonly property color broken         : "red"
        }

        readonly property color accent:  "#2f7ba3"
        readonly property color warning: "#e5b41f"

        readonly property color base0: "#242326"
        readonly property color base1: "#3f3e40"
        readonly property color base2: "#5b5a5d"
        readonly property color base3: "#79797b"
        readonly property color base4: "#99989a"
        readonly property color base5: "#bababb"
        readonly property color base6: "#dcdcdc"
        readonly property color base7: "#ffffff"

        readonly property ColorGroup group : ColorGroup {
            text:          root.color.base6
            base:          root.color.base1
            alternateBase: root.color.base2
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
