// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    title: "База данных"

    contentItem: Column {

        spacing: Sizes.base

        M.LabelPair {
            name: "Версия базы данных:"
            text: Api.groups.dbVersion()
            width: parent.width
        }
    }
}
