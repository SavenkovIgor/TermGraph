// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Atoms as A
import Theme

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
