// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Atoms as A
import StyleInfo

A.ToolButton {

    enum IconState {
        Burger = 0,
        Back
    }

    property int buttonState: BurgerButton.IconState.Burger

    icon.source: {
        switch (buttonState) {
        case BurgerButton.IconState.Burger:
            return IconPath.menu;
        case BurgerButton.IconState.Back:
            return IconPath.leftArrow;
        }

        return IconPath.menu;
    }
}
