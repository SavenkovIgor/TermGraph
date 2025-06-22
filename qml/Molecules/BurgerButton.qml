// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Atoms as A
import Theme

A.ToolButton {

    enum IconState {
        Burger = 0,
        Back
    }

    property int buttonState: BurgerButton.IconState.Burger

    icon.source: {
        switch (buttonState) {
        case BurgerButton.IconState.Burger:
            return Theme.icon.menu;
        case BurgerButton.IconState.Back:
            return Theme.icon.leftArrow;
        }

        return Theme.icon.menu;
    }
}
