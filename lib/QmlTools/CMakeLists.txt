# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (ToolsModule LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.6)

add_library (ToolsModule STATIC)

set_target_properties (ToolsModule PROPERTIES AUTOMOC ON)

set (QML_SOURCES Debug.qml Tools.qml Ruler.qml DebugFrame.qml)

set_source_files_properties (Debug.qml Tools.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module (ToolsModule
    URI Tools
    IMPORTS QtQml QtQuick
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES ${QML_SOURCES})
