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

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Atoms" as A
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors

A.PageHeader {
    id: root

    signal openMainMenu()

    property Page page: parent
    property alias title: titleLabel.text
    property alias titleVisible: titleLabel.visible
    property alias burgerState: burgerButton.buttonState

    BurgerButton {
        id: burgerButton

        buttonState: {
            if (!root.page)
                return -1;

            if (!root.page.StackView.view)
                return -1;

            return root.page.StackView.view.depth <= 1
                    ? BurgerButton.IconState.Burger
                    : BurgerButton.IconState.Back
        }

        onClicked: {
            switch (buttonState) {
            case BurgerButton.IconState.Burger:
                root.openMainMenu();
                return;
            case BurgerButton.IconState.Back:
                root.page.StackView.view.pop();
                return;
            }
        }
    }

    A.PageTitleLabel {
        id: titleLabel
        text: root.page ? root.page.title : ""
        Layout.fillWidth: true
    }
}
