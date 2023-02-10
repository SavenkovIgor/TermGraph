// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

Dialog {
    id: root

    dim: true

    x: (window.width - width) / 2
    y: (window.height / 2) - height

    palette.text: "red"
    palette.base: "black"

    standardButtons: Dialog.Yes | Dialog.No
}
