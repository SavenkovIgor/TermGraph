# Copyright © 2016-2025. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


add_library (ToolsModule STATIC)

set_target_properties (ToolsModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON)

set (QML_SOURCES
    Debug.qml
    Tools.qml
    Ruler.qml
    DebugFrame.qml
)

set_source_files_properties (
        Debug.qml
        Tools.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module (
    ToolsModule
        URI Tools
        QML_FILES ${QML_SOURCES})
