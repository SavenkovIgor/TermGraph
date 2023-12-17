# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (MoleculesModule LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.6)

qt_add_library (MoleculesModule STATIC)

set_target_properties (MoleculesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (MoleculesModule
    URI Molecules
    IMPORTS
        QtQml QtQuick QtQuick.Controls QtQuick.Controls.Material QtQuick.Shapes QtQuick.Layouts QtQuick.Window QtQuick.Templates
        Theme Tools Atoms
    IMPORT_PATH ${PROJECT_BINARY_DIR}
    QML_FILES
        AppIcon.qml
        BurgerButton.qml
        DefaultHeader.qml
        EdgeHardeningDrawer.qml
        EmptyView.qml
        HelpCard.qml
        HelpView.qml
        LabelPair.qml
        LoadingIcon.qml
        LoadingInProcess.qml
        MainAppDrawer.qml
        NodesScene.qml
        NotificationDrawer.qml
        Page.qml
        SearchDrawer.qml
        SideMenuHeader.qml
        StdDialog.qml
        LinkEditorArea.qml
        TermInfoDrawer.qml
        TextArea.qml
        TextField.qml)
