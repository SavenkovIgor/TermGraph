# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (ThemeModule LANGUAGES CXX)

find_package (Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_add_library (ThemeModule STATIC)

set_target_properties (ThemeModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

set (QML_SOURCES IconPath.qml Sizes.qml Theme.qml)

set_source_files_properties (${QML_SOURCES} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module (ThemeModule
    URI Theme
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES ${QML_SOURCES})
