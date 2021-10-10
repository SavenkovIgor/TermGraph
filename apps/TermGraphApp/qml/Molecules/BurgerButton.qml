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
