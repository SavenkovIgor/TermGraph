# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (MoleculesModule LANGUAGES CXX)

find_package (Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_add_library (MoleculesModule STATIC)

set_target_properties (MoleculesModule
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

qt_add_qml_module (MoleculesModule
    URI Molecules
    VERSION 1.0
    IMPORTS StyleInfo Tools Atoms
    RESOURCE_PREFIX "/qt/qml"
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
        TagEditorArea.qml
        TermInfoDrawer.qml
        TextArea.qml
        TextField.qml)
