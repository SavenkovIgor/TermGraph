# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (PagesModule LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_library (PagesModule STATIC)

set_target_properties (PagesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (PagesModule
    URI Pages
    IMPORTS
        QtQml QtQuick QtQuick.Controls QtQuick.Controls.Material QtQuick.Shapes QtQuick.Layouts QtQuick.Window QtQuick.Templates
        Theme Tools Atoms Molecules
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES
        AppIconsSaving.qml
        EditNode.qml
        GroupsList.qml
        MainScene.qml
        NewNode.qml
        OnBoard.qml
        Settings.qml
        TermView.qml

        Settings/Database.qml
        Settings/Help.qml
        Settings/License.qml
        Settings/NetworkSettings.qml)
