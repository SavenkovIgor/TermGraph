# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (PagesModule LANGUAGES CXX)

find_package (Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_add_library (PagesModule STATIC)

set_target_properties (PagesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (PagesModule
    URI Pages
    VERSION 1.0
    IMPORTS StyleInfo Tools Atoms Molecules
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
