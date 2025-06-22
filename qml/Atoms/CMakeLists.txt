# Copyright © 2016-2025. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


add_library (AtomsModule STATIC)

set_target_properties (AtomsModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (AtomsModule
    URI Atoms
    IMPORTS
        Theme Tools
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
        TopRoundedRect.qml
)
