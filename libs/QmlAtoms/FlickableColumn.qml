// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts

import Api

import Theme

Flickable {
    id: flick

    default property alias contentData: contentColumn.data
    property alias spacing: contentColumn.spacing

    flickableDirection: Flickable.VerticalFlick
    boundsBehavior: Flickable.StopAtBounds

    ScrollIndicator.vertical: ScrollIndicator { }

    contentWidth:  contentColumn.width
    contentHeight: contentColumn.height

    implicitWidth: contentColumn.implicitWidth
    implicitHeight: contentColumn.implicitHeight

    ColumnLayout {
        id: contentColumn

        spacing: Sizes.base
        width: flick.width
    }
}
