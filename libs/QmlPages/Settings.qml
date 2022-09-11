// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Atoms as A
import Molecules as M
import Pages as P

import Theme

M.Page {
    id: root

    title: "Настройки"

    Component { id: networkSettingsComponent; P.NetworkSettings { } }
    Component { id: databaseComponent;        P.Database { } }
    Component { id: helpPageComponent;        P.Help { } }
    Component { id: licensePageComponent;     P.License { } }

    contentItem: ListView {
        model: ListModel {
            ListElement { name: "Настройки сети" }
            ListElement { name: "База данных"    }
            ListElement { name: "Справка"        }
            ListElement { name: "Лицензия"       }
        }

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
