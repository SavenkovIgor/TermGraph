# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (AtomsModule LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS QuickControls2)

qt_add_library (AtomsModule STATIC)

set_target_properties (AtomsModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (AtomsModule
    URI Atoms
    VERSION 1.0
    IMPORTS
        QtQml QtQuick QtQuick.Controls QtQuick.Controls.Material QtQuick.Shapes QtQuick.Layouts QtQuick.Window QtQuick.Templates
        Theme Tools
    RESOURCE_PREFIX "/qt/qml"
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES
        AccentText.qml
        BottomThinLine.qml
        ComboBox.qml
        DrawerPage.qml
        Edge.qml
        FlickableColumn.qml
        Hex.qml
        MouseArea.qml
        PageHeader.qml
        PageTitleLabel.qml
        PinBar.qml
        RightThinLine.qml
        Round.qml
        RoundButton.qml
        RoundedRectangle.qml
        ScrollIndicator.qml
        SideMenuButton.qml
        SmallInfoText.qml
        SquareButton.qml
        Term.qml
        ThinLine.qml
        Timer.qml
        ToolButton.qml
        ToolTip.qml
        TopRoundedRect.qml)
