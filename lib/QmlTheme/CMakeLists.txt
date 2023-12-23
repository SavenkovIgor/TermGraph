# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (ThemeModule LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core QuickControls2 Svg)

qt_standard_project_setup(REQUIRES 6.6)

add_library (ThemeModule STATIC)

set_target_properties (ThemeModule PROPERTIES AUTOMOC ON)

set (QML_SOURCES Sizes.qml Theme.qml)

set_source_files_properties (${QML_SOURCES} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module (ThemeModule
    URI Theme
    IMPORTS QtQuick
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES ${QML_SOURCES}
    RESOURCES
        "icon/arrow-thick-left.svg"
        "icon/arrow-thick-right.svg"
        "icon/book.svg"
        "icon/check.svg"
        "icon/chevron-left.svg"
        "icon/chevron-right.svg"
        "icon/code.svg"
        "icon/cog.svg"
        "icon/crossOnCircle.svg"
        "icon/eye.svg"
        "icon/folder.svg"
        "icon/info.svg"
        "icon/list.svg"
        "icon/loop-circular.svg"
        "icon/magnifying-glass.svg"
        "icon/menu.svg"
        "icon/options.svg"
        "icon/pencil.svg"
        "icon/plus.svg"
        "icon/question-mark.svg"
        "icon/share.svg"
        "icon/spreadsheet.svg"
        "icon/trash.svg"
        "icon/warning.svg"
        "area/philosophy.svg"
        "area/math.svg"
        "area/physics.svg"
        "area/chemistry.svg"
        "area/biology.svg"
        "area/medicine.svg"
        "area/neuroscience.svg"
        "area/sociology.svg"
        "area/art.svg"
        "area/computer-science.svg"
        "area/economics.svg")

