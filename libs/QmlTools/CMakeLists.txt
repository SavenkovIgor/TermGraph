# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (ToolsModule LANGUAGES CXX)

find_package (Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_add_library (ToolsModule STATIC)

set_target_properties (ToolsModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

set (QML_SOURCES Debug.qml Tools.qml Ruler.qml DebugFrame.qml)

set_source_files_properties (Debug.qml Tools.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module (ToolsModule
    URI Tools
    VERSION 1.0
    IMPORTS QtQml QtQuick
    RESOURCE_PREFIX "/qt/qml"
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES ${QML_SOURCES})
