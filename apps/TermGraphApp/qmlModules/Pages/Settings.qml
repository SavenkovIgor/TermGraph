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
import QtQuick.Controls

import Atoms as A
import Molecules as M
import Pages as P

import StyleInfo

M.Page {
    id: root

    title: "Настройки"

    ListModel {
        id: settingsModel

        ListElement { name: "Настройки сети" }
        ListElement { name: "База данных"    }
        ListElement { name: "Справка"        }
        ListElement { name: "Лицензия"       }
    }

    Component { id: networkSettingsComponent; P.NetworkSettings { } }
    Component { id: databaseComponent;        P.Database { } }
    Component { id: helpPageComponent;        P.Help { } }
    Component { id: licensePageComponent;     P.License { } }

    contentItem: ListView {
        model: settingsModel
        delegate: A.SideMenuButton {
            width: ListView.view.width
            text: name
            display: AbstractButton.TextOnly
            onClicked: {
                const currIndex = index;
                switch (index) {
                case 0:
                    root.StackView.view.push(networkSettingsComponent);
                    break;
                case 1:
                    root.StackView.view.push(databaseComponent);
                    break;
                case 2:
                    root.StackView.view.push(helpPageComponent);
                    break;
                case 3:
                    root.StackView.view.push(licensePageComponent);
                    break;
                }
            }
        }
    }
}
